//
// Created by Killian on 28/03/2023.
//
#include <Tiles.h>

SoilTile::SoilTile(uint64_t textureIndex, sf::Vector2f position, bool collidable, const uint8_t* data, uint32_t size) : Tile(textureIndex, position, collidable)
{
    hasPlant = false; // boolean to check if there is a plant
    plantType = PlantType::NONE; // enum to define the type of plant
    isWatered = false; // boolean to check if the plant is watered
    timeElapsed = 0.f; // Amount of time elapsed
}

void SoilTile::Update(float deltaTime, GameGrid& grid, const std::shared_ptr<Player>& player)
{
    if (hasPlant == true) {
        growthTime += deltaTime;
        if (isWatered) {
            growthTime =+ deltaTime * 2.0f;

            //SetTexture("Texture de la plante arrosée");
        } else {
            //"Ne rien changer"
        }

        if (growthTime >= FULL_GROWTH_TIME) {

            //SetTexture("dernier sprite de la plante");

            // Action avec la plante qui a finit de grandir ( récolter )
            // Appliquer le sprite du sol sans plante
        }
    } else {

        //SetTexture("Texture de la première plante");
    }

}