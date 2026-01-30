#!/bin/nu

let libs = "-lraylib -lGL -lm -lpthread -ldl -lrt -lX11" | split row " "

def get_flags []: nothing -> list<string> {
  open ../compile_flags.txt | lines
}

def main [] {
  print "compile.sh: No subcommand given, running default (compile with debug settings)"
  main bdbg
}

def "main bdbg" [] { # Build (debug)
  let flags = get_flags
  clang main.c -o main -g -O1 ...$flags ...$libs
}

def "main rdbg" [] { # Run (debug)
  main bdbg ; ./main
}

def "main bweb" [] { # Build (for web)

}

def "main brelease" [] { # Build (for desktop, release mode)

}
