use bevy::prelude::*;
use bevy_rapier2d::prelude::*;

/// Mario's New Directive
struct Mario;

fn main() {
    App::build()
        .add_plugins(DefaultPlugins)
        .add_plugin(RapierPhysicsPlugin::<NoUserData>::default())
        .add_startup_system(setup_physics.system())
        .add_system(draw_mario.system())
        .run();
}

// setup a player https://bevy-cheatbook.github.io/programming/ec.html
// TODO

fn setup_physics(
    mut commands: Commands,
    asset_server: Res<AssetServer>,
    mut materials: ResMut<Assets<ColorMaterial>>
) {
    // Create the camera
    let camera = OrthographicCameraBundle::new_2d();
    
    // Create the ground
    let ground= ColliderBundle {
        shape: ColliderShape::cuboid(100.0, 0.1),
        ..Default::default()
    };
   
    // Create the sprite
    let texture_handle = asset_server.load("mario.png");
    let sprite = SpriteBundle {
        material: materials.add(texture_handle.into()),
        ..Default::default()
    };

    // Create the bouncing ball
    let rigid_body = RigidBodyBundle {
        position: Vec2::new(0.0, 10.0).into(),
        ..Default::default()
    };

    let collider = ColliderBundle {
        shape: ColliderShape::ball(0.5),
        material: ColliderMaterial {
            restitution: 0.7,
            ..Default::default()
        },
        ..Default::default()
    };
    
    commands.spawn_bundle(camera);
    commands.spawn_bundle(ground);
    commands.spawn_bundle(sprite)
        .insert(Mario)
        .insert_bundle(rigid_body)
        .insert_bundle(collider);

    
}

fn draw_mario(
    mut query: Query<(&mut Transform, &mut RigidBodyPosition), With<Mario>>
) {
    for (mut transform, body_pos) in query.iter_mut() {
        transform.translation.y = body_pos.position.translation.vector.y;
        println!("Ball altitude: {}", body_pos.position.translation.vector.y);
    }
}
