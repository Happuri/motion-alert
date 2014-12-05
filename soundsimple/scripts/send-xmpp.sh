message=$1 
set -x
m="$1 alertalert2-snd"
echo "$1""$m"  | sendxmpp -s alarm happuri@mempo.org  
echo "$1""$m"  | sendxmpp -s alarm abbys@dukgo.com  
echo "$1""$m"  | sendxmpp -s alarm rfree.m@mempo.org  
echo "$1""$m"  | sendxmpp -s alarm rfree.w@mempo.org  
echo "$m"  | sendxmpp -s alarm rfree@mempo.org  
echo "$1""$m"  | sendxmpp -s alarm rfree.mobile@jit.si  
