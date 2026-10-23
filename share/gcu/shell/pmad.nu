def --env pmad [...args: string] {
    if ($args | is-empty) {
        cd ~
    } else if ($args | length) == 1 and $args.0 == '--help' {
        print 'Usage: pmad [DIRECTORY]'
        print 'With no argument, go to HOME; with -, return to the previous directory.'
    } else if ($args | length) == 1 and $args.0 == '--version' {
        print 'pmad (GCU) 0.2.0 shell integration'
    } else {
        cd $args.0
    }
}
