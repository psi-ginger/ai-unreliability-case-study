for ($i=1; $i -le 3000; $i++) {
    $sec = Get-Random -Minimum 10 -Maximum 20
    $start = Get-Date
    $job = Start-Job -ScriptBlock { Set-Location 'C:\Data\06_Client_Goonzu\rust_Iwcgz_client'; cargo run -- --clients 3000 }
    Wait-Job $job -Timeout $sec | Out-Null
    Stop-Job $job
    Remove-Job $job
    $end = Get-Date
    Add-Content -Path 'C:\Data\06_Client_Goonzu\rust_Iwcgz_client\docs\rca\SWARM_3000_CYCLES_METRICS.csv' -Value "$i,$($start.ToString('o')),$sec,0,0,0,0,0"
}
