## To Start
```
cargo run
```
- Before you start coding, check out the [Progress](#Progress) and [Lessons Learned](#Lessons-Learned) sections

# Tech Stack
- Rendering engine: [bevy](https://bevyengine.org/)
- Physics engine: [rapier](https://rapier.rs/)

# To Release
- Remove `dynamic` feature flag from bevy's dependency in Cargo (smaller bin)
```
cargo build --release
```
Release For Web
- Research how to run on web
- Add `wasm-bindgen` feature flag to rapiers dependency

# Progress

### Todo
- Move spawn world from setup
- Research fetching directory contents
- Add open file block
- Research bevy scene changes
- Add navigate directory door
- Improve level generation
- Remove bumps when sliding

### Nice to haves
- Show framerate
- Research building a release version

### Done
- Bundle up player
- Bundle up blocks
- Generate blocks
- Setup engines
- Research best physics engine
- Research best rendering engine

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

# Level Building Ideas
- seed levels off of things in the current directory for level consistency
- randomly modify the level every time its visited
- save the level in a compressed .mario file in the current directory
