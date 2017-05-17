@echo off
SET parent=%~dp0
python "%parent%\utils\getTIandSI.py" %*
