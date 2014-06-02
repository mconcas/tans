#!/bin/bash
## Colorize the world!
bldred="\033[0;31m" # Red
txtrst="\033[0m"    # Text Reset

echo -e "$bldred Removing all *.so  $txtrst"
rm *.so 2>/dev/null
#sleep 1
echo -e "$bldred Removing all *.d $txtrst"
rm *.d 2>/dev/null
echo -e "$bldred Removing all *.root $txtrst"
rm *.root 2>/dev/null
exit 0
