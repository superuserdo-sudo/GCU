function global:pmad {
    if ($args.Count -eq 0) {
        Set-Location -LiteralPath $HOME
        return
    }
    if ($args.Count -eq 1 -and $args[0] -eq '--help') {
        Write-Output 'Usage: pmad [DIRECTORY]'
        Write-Output 'With no argument, go to HOME; with -, return to the previous directory.'
        return
    }
    if ($args.Count -eq 1 -and $args[0] -eq '--version') {
        Write-Output 'pmad (GCU) 0.2.0 shell integration'
        return
    }
    if ($args.Count -eq 1 -and $args[0] -eq '-') {
        Set-Location -
        return
    }
    if ($args.Count -gt 1) {
        Write-Error 'pmad accepts at most one directory argument.'
        return
    }
    Set-Location -LiteralPath $args[0]
}
