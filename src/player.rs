use bevy::prelude::*;
use bevy_rapier2d::prelude::*;

static PLAYER_WIDTH: u8 = 50;
static PLAYER_HEIGHT: u8 = 80;

#[derive(Bundle)]
pub struct PlayerBundle {
    pub scale: f32,
    #[bundle]
    pub sprite: SpriteBundle,
    #[bundle]
    pub body: RigidBodyBundle,
    #[bundle]
    pub collider: ColliderBundle,
}

impl Default for PlayerBundle {
    fn default() -> Self {
        Self {
            sprite: SpriteBundle {
                sprite: Sprite::new(Vec2::new(PLAYER_WIDTH as f32, PLAYER_HEIGHT as f32)),
                ..Default::default()
            },
            body: RigidBodyBundle::default(),
            collider: ColliderBundle {
                material: ColliderMaterial {
                    restitution: 0.7,
                    //friction: 0.0, TODO; test this out :)
                    ..Default::default()
                },
                ..Default::default()
            },
            scale: 1.0,
        }
    }
}

// TODO remove redundancy; combine methods with similar methods in block.rs
impl PlayerBundle {
    /// @brief Max size of 255
    pub fn new(material: Handle<ColorMaterial>, pos: Vec2, size: Vec2, scale: f32) -> Self {
        let mut player = Self { scale, ..Default::default() };
        player.set_pos(pos);
        player.set_size(size);
        player.set_texture(material);
        return player;
    }
    
    pub fn to_string(&self) -> String {
        let pos = self.collider.position.translation.vector; 
        return format!("{{ pos: [{}, {}], size: {}, scale: {} }}",
            pos.x.to_string(),
            pos.y.to_string(),
            self.sprite.sprite.size,
            self.scale.to_string(),
        );
    }

    fn set_pos(&mut self, pos: Vec2) {
        let collide_pos = pos / self.scale;
        self.collider.position = (collide_pos, 0.0).into(); 
    }

    // TODO fix this; it only resizes the collision box, not the sprite
    fn set_size(&mut self, size: Vec2) {
        let collide_dim = size / self.scale / 2.0;
        self.collider.shape = ColliderShape::cuboid(collide_dim.x, collide_dim.y);
        self.sprite.sprite.size = size;
    }

    fn set_texture(&mut self, material: Handle<ColorMaterial>) {
        self.sprite.material = material;
    }
}