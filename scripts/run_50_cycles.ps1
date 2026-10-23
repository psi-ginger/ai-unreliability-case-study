for ($i=1; $i -le 50; $i++) {
    $start = Get-Date
    $job = Start-Job -ScriptBlock { Set-Location 'C:\Data\06_Client_Goonzu\rust_Iwcgz_client'; cargo run -- --clients 3000 }
    Wait-Job $job -Timeout 65 | Out-Null
    Receive-Job $job | Out-Null
    Stop-Job $job
    Remove-Job $job
    $end = Get-Date
    Add-Content -Path 'docs/rca/SWARM_50_RUNS_METRICS.csv' -Value "$i,$($start.ToString('o')),$($end.ToString('o')),3000"
}
