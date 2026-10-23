pmad() {
    case ${1-} in
        --help)
            printf '%s\n' 'Usage: pmad [DIRECTORY]' 'Change the current shell directory. With no argument, go to HOME; with -, return to the previous directory.'
            return 0
            ;;
        --version)
            printf '%s\n' 'pmad (GCU) 0.2.0 shell integration'
            return 0
            ;;
    esac
    cd "$@"
}
