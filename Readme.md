## To Start
```
cargo run
```
- Before you start coding, check out the [Progress](#Progress) and [Lessons Learned](#Lessons-Learned) sections

# Tech Stack
- Rendering engine: [bevy](https://bevyengine.org/)
- Physics engine: [rapier](https://rapier.rs/)

# To Release
- remove `dynamic` feature flag from bevy's dependency in Cargo (smaller bin)
Release For Web
- add `wasm-bindgen` feature flag to rapiers dependency 

# Progress

## Todo
- Bundle up player

## Done
- Bundle up blocks
- Generate blocks
- Connect engines
- Find the best physics engine
- Find the best rendering engine

## Nice to haves
- hmm...

# Lessons Learned
```
// to add a package
cargo install cargo-edit
cargo add [package name]

// to hot-reload
cargo install cargo-watch
cargo watch -w src -w assets -w Cargo.toml -x run

// to enable stacktrace 
export RUST_BACKTRACE=1

// Use nightly because it supports monomorphization

// for performance install a lld llvm
brew install michaeleisel/zld/zld // on mac (might require prereq lib)
```
