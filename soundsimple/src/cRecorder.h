/*
 * cRecorder.h
 *
 *  Created on: 26 lis 2014
 */

#ifndef CRECORDER_H_
#define CRECORDER_H_

#include "libs.h"
#include "cSound.h"
#include "cSoundFrame.h"
#include "cSend.h"
#include "cFile.h"
#include <boost/circular_buffer.hpp>

#define CBUFF_SIZE 200
#define STEREO 2
#define MONO 1
#define EVENT_TIME 10
#define FIRST_FILES_TIME 5
#define NEXT_FILES_TIME 10
#define SAVING_LOG "saving_buff"

const std::string recDirName = "recordings/";

class cAlarm {
};

class cAlarmSoundRecorder: public sf::SoundBufferRecorder {
public:
	void setSimulationMode(bool simulationMode = false){
		this->simulationMode = simulationMode;
		_dbg3(simulationMode);
	}
private:
	boost::circular_buffer<cSoundFrame> mRawBuffer = boost::circular_buffer<
			cSoundFrame>(CBUFF_SIZE);

	std::chrono::steady_clock::time_point mAlarmLastTime;
	std::chrono::steady_clock::duration diffToAlarm;

	bool isEvent = false;
	bool savedMinusFile = false;
	bool simulationMode = false;
	unsigned int mSavedFiles = 0;
	std::string message = "";

	virtual bool OnStart() {
		std::cout << "Start sound recorder" << std::endl;
		return true;
	}

	void createAndSaveFrameToCBuff(const sf::Int16* Samples, std::size_t SamplesCount) {
		cSoundFrame mSoundFrame(Samples, SamplesCount);
		mRawBuffer.push_back(mSoundFrame);

	}

	std::vector<sf::Int16> mergeCBuff() {
		std::vector<sf::Int16> samplesFromCBuff;
		samplesFromCBuff.reserve(1000000);
		time_t tt;
		tt = std::chrono::system_clock::to_time_t(mRawBuffer.at(0).getStartTime());
		for (auto sample : mRawBuffer) {
			auto chunk = sample.getSamplesVec();
			for (auto element : chunk)
				samplesFromCBuff.push_back(element);
		}
		return samplesFromCBuff;
	}

	void saveBuffToFile(const sf::Int16* Samples, std::size_t SamplesCount, unsigned int SampleRate, std::string date, std::string mess) {
		assert(Samples != nullptr && SamplesCount > 0);

		sf::SoundBuffer buff;
		buff.LoadFromSamples(Samples, SamplesCount, MONO, SampleRate);
		assert(buff.GetDuration() != 0);

		_dbg2_c(SAVING_LOG, "size of samples(get samples): " << sizeof(buff.GetSamples()));
		_info_c(SAVING_LOG, "samples count: " << buff.GetSamplesCount() << ", duration: " << buff.GetDuration());
		std::string filename = cFile::getFilename(date);
		if (!buff.SaveToFile(filename)) _erro(filename << " not saved :( ");
		else {
			assert(this->message != "");
			_note("File saved " << filename);
			_note_c(SAVING_LOG, "File saved " << filename);
			cSend::sendMailNotificationMessage(mess, filename);
		}

		mRawBuffer.clear();
	}


	void handleAlarm(std::shared_ptr<cSound> &sound) {
		mAlarmLastTime = std::chrono::steady_clock::now();
		assert(!mRawBuffer.empty());
		this->message = sound->getMessage();
		_info_c(SAVING_LOG, "message: " << message);
	}

	void handleEvent(std::shared_ptr<cSound> &sound) {
		unsigned int SampleRate = GetSampleRate();
		diffToAlarm = std::chrono::steady_clock::now() - mAlarmLastTime;
		if (diffToAlarm < std::chrono::seconds(EVENT_TIME)) {
			isEvent = true;
			auto vecOfSamples = mergeCBuff();

			// saving last 20s
			if (!savedMinusFile) {
				_dbg2_c(SAVING_LOG, "saving 20s file");
				saveBuffToFile(vecOfSamples.data(), vecOfSamples.size(), SampleRate, sound->currentDateTime(), message);
				savedMinusFile = true;
			}

			// saving 3 files (1s)
			else if (mSavedFiles < 3 && mRawBuffer.size() >= FIRST_FILES_TIME * 10) {
				_dbg2_c(SAVING_LOG, "saving 1s file");
				++mSavedFiles;
				saveBuffToFile(vecOfSamples.data(), vecOfSamples.size(), SampleRate, sound->currentDateTime(), message);

			}

			// other files (10s)
			else if (mSavedFiles >= 3 && mRawBuffer.size() >= NEXT_FILES_TIME * 10) {
				_dbg2_c(SAVING_LOG, "saving 10s file");
				saveBuffToFile(vecOfSamples.data(), vecOfSamples.size(), SampleRate, sound->currentDateTime(), message);
			}
		}
		else {
			isEvent = false;
			savedMinusFile = false;
			mSavedFiles = 0;
			this->message = "";
		}
	}

	/**
	 * Audio samples are provided to the onProcessSamples function every 100 ms.
	 * This is currently hard-coded into SFML and you can't change that
	 * (unless you modify SFML itself).
	 *
	 * SamplesCount = size of Samples
	 */
	virtual bool OnProcessSamples(const sf::Int16* Samples, std::size_t SamplesCount) {
		unsigned int SampleRate = GetSampleRate();
		createAndSaveFrameToCBuff(Samples, SamplesCount);

		auto sound = std::make_shared<cSound>(isEvent);
		if (simulationMode) sound->setSimulationMode();

		auto wasAlarm = sound->ProccessRecording(Samples, SamplesCount, SampleRate);
		if (wasAlarm) handleAlarm(sound);

		handleEvent(sound);

		// return true to continue the capture, or false to stop it
		return true;
	}

	virtual void OnStop() {
		std::cout << "Stop sound recorder" << std::endl;
	}

};

class cRecorder {
public:
	cRecorder();
	virtual ~cRecorder();
	void startRecording();
	void setSimulationMode();

	const bool fromMicrophoneMode;

private:
	cAlarmSoundRecorder Recorder;

	void waitForExitKey();
};

#endif /* CRECORDER_H_ */
