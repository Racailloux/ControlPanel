@echo off
setlocal

echo Searching for Control Panel board...
for /f "tokens=1* delims==" %%I in ('wmic path win32_pnpentity get caption  /format:list ^| find "Arduino Leonardo"') do (
    echo Control panel found : %%~J
    call :resetCOM "%%~J"
    goto :end
)
echo == Control Panel was not found.
pause
goto :end

:continue
echo Waiting for Control Panel to reboot...
timeout /t 5 /nobreak > NUL

echo Searching for Control Panel bootloader...
for /f "tokens=1* delims==" %%I in ('wmic path win32_pnpentity get caption  /format:list ^| find "Arduino Leonardo bootloader"') do (
    echo Board Arduino Leonardo found : %%~J
    call :setCOM "%%~J"
    goto :end
)
echo == Control Panel bootloader was not found.
pause
goto :end

:flash
echo Flashing Control Panel...
avrdude -v -C./avrdude.conf -patmega32u4 -cavr109 -P%port% -b57600 -D -V -Uflash:w:./firmware.hex:i
echo == Flash completed successfully!
pause
goto :end


:resetCOM <WMIC_output_line>
:: sets _COM#=line
setlocal
set "str=%~1"
set "num=%str:*(COM=%"
set "num=%num:)=%"
set port=COM%num%
echo resetCOM : %port%
mode %port%: BAUD=1200 parity=N data=8 stop=1
goto :continue


:setCOM <WMIC_output_line>
:: sets _COM#=line
setlocal
set "str=%~1"
set "num=%str:*(COM=%"
set "num=%num:)=%"
set port=COM%num%
echo setCOM %port%
goto :flash

:end
