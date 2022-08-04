use bevy::prelude::*;
use bevy_rapier2d::prelude::*;

static BLOCK_SIZE: u8 = 40;

#[derive(Bundle)]
pub struct BlockBundle {
    pub scale: f32,
    #[bundle]
    pub sprite: SpriteBundle,
    #[bundle]
    pub body: RigidBodyBundle,
    #[bundle]
    pub collider: ColliderBundle,
}

impl Default for BlockBundle {
    fn default() -> Self {
        Self {
            sprite: SpriteBundle {
                sprite: Sprite::new(Vec2::new(BLOCK_SIZE as f32, BLOCK_SIZE as f32)),
                ..Default::default()
            },
            body: RigidBodyBundle {
                body_type: RigidBodyType::Static,
                ..Default::default()
            },
            collider: ColliderBundle {
                material: ColliderMaterial {
                    friction: 0.0,
                    ..Default::default()
                },
                ..Default::default()
            },
            scale: 1.0,
        }
    }
}

impl BlockBundle {
    /// @brief Max size of 255
    pub fn new(material: Handle<ColorMaterial>, pos: Vec2, size: u8, scale: f32) -> Self {
        let mut block = Self { scale, ..Default::default() };
        block.set_pos(pos);
        block.set_size(size);
        block.set_texture(material);
        return block;
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
    /// @brief Max size of 255
    fn set_size(&mut self, size: u8) {
        let dim = Vec2::new(size as f32, size as f32);
        let collide_dim = dim / self.scale / 2.0;
        self.collider.shape = ColliderShape::cuboid(collide_dim.x, collide_dim.y);
        self.sprite.sprite.size = dim;
    }

    fn set_texture(&mut self, material: Handle<ColorMaterial>) {
        self.sprite.material = material;
    }
}
