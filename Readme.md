Todo
- Keep reading the bevy book

Done
- Find the best rendering engine
- Find the best physics engine

Nice to haves
- hot reloading

Tech Stack
- Rendering engine: [bevy](https://bevyengine.org/)
- Physics engine: [rapier](https://rapier.rs/)

Start
```cargo run```

Release
- remove `dynamic` feature flag from bevy's dependency in Cargo (smaller bin)
Release For Web
- add `wasm-bindgen` feature flag to rapiers dependency 

Lessons Learned
```
// to add a package
cargo install cargo-edit
cargo add [package name]

// Use nightly because it supports monomorphization

// for performance install a lld llvm
brew install michaeleisel/zld/zld // on mac
```