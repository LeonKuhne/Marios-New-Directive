mod block;
use block::BlockBundle;

use bevy::prelude::*;
use bevy_rapier2d::prelude::*;
use rand::{thread_rng, Rng};

/// Mario's New Directive
//#[allow(warnings)] 

struct Mario {
    jump_force: f32,
    speed: f32,
}

fn main() {
    App::build()
        .insert_resource(WindowDescriptor {
            title: "Mariovich's Directive".to_string(),
            ..Default::default()
        })
        .add_plugins(DefaultPlugins)
        .add_plugin(bevy_winit::WinitPlugin::default())
        //.add_plugin(bevy_wgpu::WgpuPlugin::default())
        .add_plugin(RapierPhysicsPlugin::<NoUserData>::default())
        .add_startup_system(setup.system())
        .add_system(move_mario.system())
        .run();
}

fn setup(
    mut commands: Commands,
    asset_server: Res<AssetServer>,
    mut materials: ResMut<Assets<ColorMaterial>>,
    mut config: ResMut<RapierConfiguration>,
) {
    // Configure
    let mario = Mario{ jump_force: 600.0, speed: 20.0 };
    let player_pos = Vec2::new(0.0, 0.0);
    let player_dim = Vec2::new(50.0, 80.0);

    // Apply gravity
    config.scale = 100.0;
    //config.gravity = Vector::new(0.0, -1.0); // scaled pixels per second

    /*
    // Create the World
    */

    // Create the camera
    let mut camera = OrthographicCameraBundle::new_2d();
    camera.global_transform.translation = Vec3::new(0.0, 0.0, 100.0);
    commands.spawn_bundle(camera);
 
    // Spawn some blocks
    let grid_size: u8 = 50;
    let width_range = 15;
    let height_range = 6;
    let height_offset= -8;
    let texture_file = "block.png";
    for x in -width_range..width_range{
        for y in height_offset..(height_offset + height_range){
            let rand_height = thread_rng().gen_range(0..height_range);
            if y >= rand_height + height_offset {
                let pos = Vec2::new(x as f32 * grid_size as f32, y as f32 * grid_size as f32);
                let texture_handle = asset_server.load(texture_file);
                let material = materials.add(texture_handle.into());
                // TODO eliminate random bumps during movement and to remove +1 on gridsize
                // -> idealy combine static blocks to create smooth polygon
                let block = BlockBundle::new(material, pos, grid_size+1, config.scale);
                commands.spawn_bundle(block)
                    .insert(ColliderPositionSync::Discrete); // link the physics to sprite
                    //.insert(ColliderDebugRender::with_id(0));
            }
        }
    }
    
    /*
    // Create the Player
    // TODO bundle up the player like you did the block
    */
    
    // Create the sprite
    let texture_handle = asset_server.load("mario2.png");
    let sprite = SpriteBundle {
        material: materials.add(texture_handle.into()),
        sprite: Sprite::new(player_dim),
        ..Default::default()
    };
    let rigid_body = RigidBodyBundle::default();
    
    // Setup collision box
    let collide_dim = player_dim / config.scale / 2.0;
    let player_collide_pos = player_pos / config.scale;
    let collider = ColliderBundle {
        shape: ColliderShape::cuboid(collide_dim.x, collide_dim.y),
        position: (player_collide_pos, 0.0).into(),
        material: ColliderMaterial {
            restitution: 0.7,
            ..Default::default()
        },
        ..Default::default()
    };
    commands.spawn()
        .insert_bundle(sprite)
        .insert_bundle(rigid_body)
        .insert_bundle(collider)
        .insert(ColliderPositionSync::Discrete) // link the physics to sprite
        //.insert(ColliderDebugRender::with_id(0))
        .insert(mario);

    // Spawn a block to hit
    let pos = Vec2::new(5 as f32 * grid_size as f32, 2 as f32 * grid_size as f32);
    let texture_handle = asset_server.load(texture_file);
    let material = materials.add(texture_handle.into());
    let block = BlockBundle::new(material, pos, grid_size as u8, config.scale);
    commands.spawn_bundle(block)
        .insert(ColliderPositionSync::Discrete); // link the physics to sprite
        //.insert(ColliderDebugRender::with_id(0));
}

fn move_mario(
    keys: Res<Input<KeyCode>>,
    config: ResMut<RapierConfiguration>,
    mut player: Query<(&Mario, &mut RigidBodyVelocity)>,
) {
    for (mario, mut body_vel) in player.iter_mut() {
        // jump
        if keys.pressed(KeyCode::Up) {
            body_vel.linvel.y = mario.jump_force / config.scale;
        }

        let down = keys.pressed(KeyCode::Down);
        let left = keys.pressed(KeyCode::Left);
        let right = keys.pressed(KeyCode::Right);

        // calculate movement delta
        let x_axis = -(left as i8) + right as i8;
        let y_axis = -(down as i8);
        let mut delta = Vector::new(x_axis as f32, y_axis as f32);
        if delta != Vector::zeros() {
            delta *= mario.speed / delta.magnitude() / config.scale;
        }

        body_vel.linvel += delta;
    }
}