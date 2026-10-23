function pmad --description 'Change the current directory using GCU'
    if test (count $argv) -eq 1
        switch $argv[1]
            case --help
                printf '%s\n' 'Usage: pmad [DIRECTORY]' 'With no argument, go to HOME; with -, return to the previous directory.'
                return 0
            case --version
                printf '%s\n' 'pmad (GCU) 0.2.0 shell integration'
                return 0
        end
    end
    builtin cd $argv
end
