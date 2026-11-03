
@echo off
setlocal enabledelayedexpansion
set LIC_PREFIX=microchip

echo Detecting Operating System
ver | findstr /i "5\.1\." > nul
IF %ERRORLEVEL% EQU 0 goto ver_XP
ver | findstr /i "5\.2\." > nul
IF %ERRORLEVEL% EQU 0 goto ver_XP
ver | findstr /i "6\.0\." > nul
IF %ERRORLEVEL% EQU 0 goto ver_Vista
ver | findstr /i "6\.1\." > nul
IF %ERRORLEVEL% EQU 0 goto ver_Win7
ver | findstr /i "6\.2\." > nul
IF %ERRORLEVEL% EQU 0 goto ver_Win7
ver | findstr /i "6\.3\." > nul
IF %ERRORLEVEL% EQU 0 goto ver_Win7
ver | findstr /i "6\.4\." > nul
IF %ERRORLEVEL% EQU 0 goto ver_Win7
ver | findstr /i "10\.0\." > nul
IF %ERRORLEVEL% EQU 0 goto ver_Win7
goto warn_and_exit

:ver_Win7
echo OS Version: Windows 7/8/8.1/10/11
goto set_Vista_Win7

:ver_Vista
echo OS Version: Windows Vista
goto set_Vista_Win7

:ver_XP
echo OS Version: Windows XP
goto set_XP

:warn_and_exit
echo Operating system:
ver
echo ... is not supported.
goto end

:set_XP
set DIR="%ALLUSERSPROFILE%\Application Data\Microchip\xclm\license\"
goto makelicense

:set_Vista_Win7
set DIR="%PROGRAMDATA%\Microchip\xclm\license\"
goto makelicense

:makelicense
if not exist %DIR% goto nodirfound
    for /L %%G in (1,1,1000) do (
set FILE=%DIR%%LIC_PREFIX%-%%G.lic
if not exist "!FILE!" goto endloop
)

:endloop
echo Creating license file %FILE%
echo This license created by Microchip Web Activation >> %FILE%
echo Please do not edit the content of this file >> %FILE%
echo LICENSE microchip swxc8-pro 1.0 permanent uncounted hostid=ANY >> %FILE%
echo  options=swxc8-pro-permanent _ck=921e6e2907 sig=^"60PG4538QYQ89WAAEHMC >> %FILE%
echo  1B983859EM7P6UKA4XR22M0D0URHFFE42TF1VST73C8ECG6X6U6FFY9G^" >> %FILE%
echo LICENSE microchip swxc16-pro 1.0 permanent uncounted hostid=ANY >> %FILE%
echo  options=swxc16-pro-permanent _ck=c11e9d8c62 sig=^"60Q04580SDDMY6NJWT4 >> %FILE%
echo  7QMTYS7AMDSUFPQNS1M7908AG1UNEK37YR8T70PG5PJ5SKY45BXF4N2C7E^" >> %FILE%
echo LICENSE microchip swxc32-cpp 1.0 permanent uncounted hostid=ANY >> %FILE%
echo  options=swxc32-pro-permanent _ck=b11e9bc75e sig=^"60Q04580M1B1VFF0U1Q >> %FILE%
echo  7WFXXPK4DHBEYQF33Q5Y108AG14H2N4MPQG1VX49XF04WQTQ3PXR1B65SJ^" >> %FILE%
echo LICENSE microchip swxc8-fs 1.0 permanent uncounted hostid=ANY >> %FILE%
echo  options=swxc8-fs-permanent _ck=1a1ef64a59 sig=^"60PG4580WFWJCRM56NM9T >> %FILE%
echo  VSSAJ9WX207VPHW3MPY08A3MPKWV373DUNCGQGDW2ARPVYU2Q2D2UAG^" >> %FILE%
echo LICENSE microchip swxc16-fs 1.0 permanent uncounted hostid=ANY >> %FILE%
echo  options=swxc16-fs-permanent _ck=491e253e02 sig=^"60PG4580UXX3BN2VMXCY >> %FILE%
echo  2DGQD68SJPV4VRV42M9P08A02XJ05PFRFCUYV99JGB0H3WF7NFHJKN40^" >> %FILE%
echo LICENSE microchip swxc32-fs 1.0 permanent uncounted hostid=ANY >> %FILE%
echo  options=swxc32-fs-permanent _ck=471e23ac2a sig=^"60Q04580WQH2AYXMBGA9 >> %FILE%
echo  9JX44W66TK9GDHEYDAWM08AG1T6AH4CVH98XBXCUE2Y3R6F3JCKHW1EQR^" >> %FILE%
echo LICENSE microchip swanaly-ts 1.0 permanent uncounted hostid=ANY >> %FILE%
echo  options=swanaly-ts-permanent _ck=2a1e0655f9 sig=^"60P0453MAP8FDWNPY7D >> %FILE%
echo  RGGS1H4RJBTF4FNDBYUG22HXH8RX8EQPJG0GPWGA51TU63SA01WHUS8^" >> %FILE%


goto end

:nodirfound
echo The folder %DIR% does not exist.
echo Please install an XC compiler, then rerun the script.
goto end

:end
pause
