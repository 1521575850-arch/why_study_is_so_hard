@echo off
chcp 936 >nul
powershell -ExecutionPolicy Bypass -File "%~dp0upload.ps1"
