# Crack
 
Crack Engine is a custom 2D action game engine developed in C++ using OpenGL.

This engine is built with data-driven development, allowing developers to change large amounts of content without having to compile any script.

With a robust animation system, fighting game enthusiasts can take advantage of in-depth features like per-frame timings, and accurate hitboxes and hurtboxes.

In-game content is driven using xml files that can be edited manually or using our in-engine tools, with all the features needed to execute a complex fighting game character, a challenging boss battle or an in-depth RPG story. 

## Todo
### System
- [ ] Stage definitions (Stage elements, names, thumbnails, music etc.)
- [ ] Input remapping, support for multiple controllers
- [x] imGui diagnostics and tools (partial)
- [ ] Data bundling (One file including all images etc.)
### Gameplay
- [x] Command buffer system (double-taps, quarter circles, etc.) (partial)
- [x] Velocity / Gravity for jumping
- [x] Hitbox-Hurtbox creation and colission (in-progress)
- [ ] Projectiles
- [ ] Grabs
### Graphics
- [ ] Effects and hitsparks spawning
### Sound
- [x] Sound implementation using Fmod (music, sound effects on command) (partial)
- [ ] Metadata loading (Display current song in menu)
- [ ] Localisation features
### UI
- [ ] Fonts
- [ ] Background UI Elements
- [ ] Interactable UIs
- [ ] Character/Stage select screen
### Project 1 - Fighting game content
- [x] Jumping
- [x] Dashing
- [x] Normal Moves
- [ ] Grabs
- [x] Specials
- [ ] Supers
