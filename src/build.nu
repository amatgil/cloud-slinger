#!/bin/nu

let libs = "-lraylib -lGL -lm -lpthread -ldl -lrt -lX11" | split row " "
let flags = open ../compile_flags.txt | lines

def main [] {
  print "build.sh: No subcommand given, running default (compile with debug settings)"
  main build
}

def "main build" [] {
  print "Building in debug mode (desktop)"
  clang main.c -o main -g -O1 ...$flags ...$libs
  print "Done"
}

def "main build --release" [] {
  print "Building in release mode (desktop)"
  clang main.c -o main -O3 ...$flags ...$libs
  print "Done"
}

def "main run" [] {
  print "Running in debug mode"
  main build ; ./main
}

def "main bweb" [] { # Build (for web)

}
