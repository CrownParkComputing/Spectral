for %%i in (*.pal) do (
	bin2c.exe %%i %%~ni.h %%~ni
)
