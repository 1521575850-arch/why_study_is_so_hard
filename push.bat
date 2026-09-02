@echo off
chcp 65001 >nul
cd /d D:\study\git_push\why_study_is_so_hard
echo ========================================
echo   GitHub 一键上传
echo ========================================
git add -A
echo.
set /p msg=本次更新说明(直接回车用自动时间): 
if "%msg%"=="" set msg=update %date% %time%
git commit -m "%msg%"
git push
echo.
echo ===== 上传完成! 刷新 GitHub 页面即可查看 =====
pause
