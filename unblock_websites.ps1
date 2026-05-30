# Unblock all websites by removing Focus App entries from hosts file
$hostsPath = "C:\Windows\System32\drivers\etc\hosts"

Write-Host "Reading hosts file..." -ForegroundColor Yellow
$content = Get-Content $hostsPath

Write-Host "Removing Focus App blocking entries..." -ForegroundColor Yellow
$cleaned = @()
$skip = $false
$removed = 0

foreach ($line in $content) {
    if ($line -match '# Focus App - Website Blocking') {
        $skip = $true
        continue
    }
    if ($line -match '# End Focus App Blocking') {
        $skip = $false
        $removed++
        continue
    }
    if (-not $skip) {
        $cleaned += $line
    }
}

Write-Host "Writing cleaned hosts file..." -ForegroundColor Yellow
$cleaned | Set-Content $hostsPath

Write-Host "Flushing DNS cache..." -ForegroundColor Yellow
ipconfig /flushdns | Out-Null

Write-Host "`nSuccess! Removed $removed blocking section(s)." -ForegroundColor Green
Write-Host "All websites are now unblocked!" -ForegroundColor Green
