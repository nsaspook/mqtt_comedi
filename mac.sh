#!/bin/bash

# License filename prefix
prefix="microchip"

# Operating system name
os=`uname -s`

# Pick license file location based on OS name
dir=""
if [ `echo $os | awk '{ print $0 ~ "CYGWIN" }'` = "1" ]; then
    if [ `echo $os | awk '{ print $0 ~ "5.2" }'` = "1" ]; then
        dir="$ALLUSERPROFILE/Application Data/Microchip/xclm/license/"
    else
        dir="$ProgramData/Microchip/xclm/license/"
    fi
    slashforward='s/\/\//g'
    dir=`echo $dir | sed -e "$slashforward"`
elif [ `echo $os | awk '{ print $0 ~ "Linux" }'` = "1" ]; then
    dir='/opt/microchip/xclm/license/'
elif [ `echo $os | awk '{ print $0 ~ "Darwin" }'` = "1" ]; then
    dir='/Library/Application Support/Microchip/xclm/license/'
fi

# Announce OS version detected
echo 'Detected operating system: '$os

# Couldn't find a supported operating system ?
if [ -z "$dir" ]; then
    echo 'Operating system '$os' is not supported.'
    exit 1
fi

# Check for existence of license directory
if [ ! -d "$dir" ]; then
    echo 'The folder/directory '$dir' does not exist.'
    echo 'Please install an XC compiler, then rerun the script.'
    exit 1
fi

# Choose license filename
i='1'
while [ `expr $i \< 1000` = "1" ]; do
    file="$dir$prefix-$i.lic"
    if [ ! -f "$file" ]; then
        break
    fi
    i=`expr $i + 1`
done
# Announce license filename
echo
echo "Creating license file $file"

# Write license file
(cat <<_EOF_
This license created by Microchip Web Activation
Please do not edit the content of this file
_EOF_
) > "$file"
(cat <<_EOF_
LICENSE microchip swxc8-pro 1.0 permanent uncounted hostid=ANY
  options=swxc8-pro-permanent _ck=921e6e2907 sig="60PG4538QYQ89WAAEHMC
  1B983859EM7P6UKA4XR22M0D0URHFFE42TF1VST73C8ECG6X6U6FFY9G"
LICENSE microchip swxc16-pro 1.0 permanent uncounted hostid=ANY
  options=swxc16-pro-permanent _ck=c11e9d8c62 sig="60Q04580SDDMY6NJWT4
  7QMTYS7AMDSUFPQNS1M7908AG1UNEK37YR8T70PG5PJ5SKY45BXF4N2C7E"
LICENSE microchip swxc32-cpp 1.0 permanent uncounted hostid=ANY
  options=swxc32-pro-permanent _ck=b11e9bc75e sig="60Q04580M1B1VFF0U1Q
  7WFXXPK4DHBEYQF33Q5Y108AG14H2N4MPQG1VX49XF04WQTQ3PXR1B65SJ"
LICENSE microchip swxc8-fs 1.0 permanent uncounted hostid=ANY
  options=swxc8-fs-permanent _ck=1a1ef64a59 sig="60PG4580WFWJCRM56NM9T
  VSSAJ9WX207VPHW3MPY08A3MPKWV373DUNCGQGDW2ARPVYU2Q2D2UAG"
LICENSE microchip swxc16-fs 1.0 permanent uncounted hostid=ANY
  options=swxc16-fs-permanent _ck=491e253e02 sig="60PG4580UXX3BN2VMXCY
  2DGQD68SJPV4VRV42M9P08A02XJ05PFRFCUYV99JGB0H3WF7NFHJKN40"
LICENSE microchip swxc32-fs 1.0 permanent uncounted hostid=ANY
  options=swxc32-fs-permanent _ck=471e23ac2a sig="60Q04580WQH2AYXMBGA9
  9JX44W66TK9GDHEYDAWM08AG1T6AH4CVH98XBXCUE2Y3R6F3JCKHW1EQR"
LICENSE microchip swanaly-ts 1.0 permanent uncounted hostid=ANY
  options=swanaly-ts-permanent _ck=2a1e0655f9 sig="60P0453MAP8FDWNPY7D
  RGGS1H4RJBTF4FNDBYUG22HXH8RX8EQPJG0GPWGA51TU63SA01WHUS8"
_EOF_
) >> "$file" 
