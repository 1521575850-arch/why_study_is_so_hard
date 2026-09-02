$ErrorActionPreference = "Continue"
$repo = "D:\study\git_push\why_study_is_so_hard"
Set-Location $repo

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  一键同步并上传到 GitHub" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# 1. 同步虚拟机代码
Write-Host "[1/4] 正在从虚拟机同步代码..." -ForegroundColor Yellow
$vmDir = "$repo\虚拟机代码"
if (Test-Path $vmDir) { Remove-Item $vmDir -Recurse -Force }
New-Item -ItemType Directory -Path $vmDir | Out-Null
scp -r -o BatchMode=yes -o ConnectTimeout=10 wiyyj@192.168.190.129:/home/wiyyj/IO "$vmDir\" 2>$null
if (Test-Path "$vmDir\IO") {
  Get-ChildItem "$vmDir\IO" -Force | Move-Item -Destination $vmDir -Force
  Remove-Item "$vmDir\IO" -Recurse -Force
}

# 2. 同步主机代码
Write-Host "[2/4] 正在同步主机代码..." -ForegroundColor Yellow
$pcDir = "$repo\主机代码"
if (Test-Path $pcDir) { Remove-Item $pcDir -Recurse -Force }
Copy-Item "D:\study\study_program" $pcDir -Recurse

# 3. 清理编译产物
Write-Host "[3/4] 正在清理编译产物..." -ForegroundColor Yellow
Get-ChildItem $pcDir -Recurse -File -ErrorAction SilentlyContinue | Where-Object { $_.Extension -eq ".exe" -or $_.Extension -eq ".o" -or $_.Extension -eq ".obj" } | Remove-Item -Force
Get-ChildItem $vmDir -Recurse -File -ErrorAction SilentlyContinue | Where-Object { $_.Extension -eq "" } | Remove-Item -Force

# 4. 提交并上传
Write-Host "[4/4] 正在提交并上传..." -ForegroundColor Yellow
git add -A
$msg = Read-Host "请输入本次更新说明(直接回车用自动时间)"
if ([string]::IsNullOrWhiteSpace($msg)) { $msg = "update $(Get-Date -Format 'yyyy-MM-dd HH:mm')" }
git commit -m $msg 2>&1 | Out-Null
git push
if ($LASTEXITCODE -ne 0) {
  Write-Host ""
  Write-Host "!! 上传失败：网络连不上 GitHub(国内访问不稳定)。" -ForegroundColor Red
  Write-Host "代码已安全保存在本地仓库，不会丢失。" -ForegroundColor Yellow
  Write-Host "稍等几分钟再双击本工具重试即可，或等网络恢复。" -ForegroundColor Yellow
  Read-Host "按回车退出"
  exit
}
Write-Host ""
Write-Host "===== 上传完成! 刷新 GitHub 页面即可查看 =====" -ForegroundColor Green
Read-Host "按回车退出"