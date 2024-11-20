set -e
#make clean
rivemu -quiet -no-window -sdk -workspace -exec make
rivemu -workspace -exec ./rush levels.txt
