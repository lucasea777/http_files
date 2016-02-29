make
wget -O horse.mp3 http://www.w3schools.com/html/horse.mp3 &> /dev/null
correct=$(md5sum horse.mp3 | cut -d' ' -f1)
rm horse.mp3;
./httpget www.w3schools.com /html/horse.mp3 horse.mp3 &> /dev/null
downloaded=$(md5sum horse.mp3 | cut -d' ' -f1)
if [[ $downloaded == $correct ]]; then echo 'OK'; else echo 'ERROR'; fi

