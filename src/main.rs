mod block;
mod player;
use block::BlockBundle;
use player::PlayerBundle;

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
    // Apply gravity
    config.scale = 100.0;
    //config.gravity = Vector::new(0.0, -1.0); // scaled pixels per second

    // Configure Block
    let block_material = materials.add(
        asset_server.load("block.png").into());

    // Configure Player
    let mario = Mario{ jump_force: 600.0, speed: 20.0 };
    let player_pos = Vec2::new(0.0, 0.0);
    let player_size = Vec2::new(50.0, 80.0);
    let player_material = materials.add(
        asset_server.load("mario2.png").into());
    
    /*
    // Create the World
    */

    // Create the camera
    commands.spawn_bundle(OrthographicCameraBundle::new_2d());
 
    // Spawn some blocks
    let grid_size: u8 = 50;
    let width_range = 15;
    let height_range = 6;
    let height_offset= -8;
    for x in -width_range..width_range{
        for y in height_offset..(height_offset + height_range){
            let rand_height = thread_rng().gen_range(0..height_range);
            if y >= rand_height + height_offset {
                // TODO remove redundancy; block_material should only be declared once
                let block_material = materials.add(
                    asset_server.load("block.png").into());
                let pos = Vec2::new(x as f32 * grid_size as f32, y as f32 * grid_size as f32);
                // TODO eliminate random bumps during movement and to remove +1 on gridsize
                // -> idealy combine static blocks to create smooth polygon
                let block = BlockBundle::new(block_material, pos, grid_size+1, config.scale);
                commands.spawn_bundle(block)
                    .insert(ColliderPositionSync::Discrete); // link the physics to sprite
                    //.insert(ColliderDebugRender::with_id(0));
            }
        }
    }
    
    // Spawn the Player
    let player = PlayerBundle::new(player_material, player_pos, player_size, config.scale);
    commands.spawn()
        .insert_bundle(player)
        .insert(ColliderPositionSync::Discrete) // link the physics to sprite
        .insert(ColliderDebugRender::with_id(0))
        .insert(mario);

    // Spawn a block to hit
    let pos = Vec2::new(5 as f32 * grid_size as f32, 2 as f32 * grid_size as f32); 
    let block = BlockBundle::new(block_material, pos, grid_size as u8, config.scale);
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