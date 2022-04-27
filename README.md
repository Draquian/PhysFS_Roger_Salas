# Asset Zip Management with PhysFS

## About me
I am Roger Salas, student of Video Games Degree by UPC at CITM. This content is generated for the second year’s subject Project 2, under supervision of Pedro Omedas.


## About the project
This project is about loading assets from a compressed archive using the librerys PhysFS, SDL and PugiXML.

Types of compressed archive:
  - ZIP
  - 7Z
  - ISO
  - PAK
  - GRP
  - HOG
  - MVL
  - WAD
  - VDF
  - SLB


PhysFS to open the Zip file and achieve the main goal.

  - Some functions to do this:
      - PHYSFS_init( );
      - PHYSFS_deinit( );
      - PHYSFS_mount( );
      - PHYSFS_addToSearchPath(const char “Path”, 1);
      - PHYSFS_exist();
      - PHYSFS_mount(const char “Path”);
      - PHYSFS_getLastError( );
      - PHYSFS_close( );
      - PHYSFS_openRead(const char “fileName” );
      - PHYSFS_fileLength(PHYSFS_file* file);
      - PHYSFS_read();


SDL to create the windows to show a texture, play a sound, and use the fils from the Zip file.

  - Some functions to do this:
    - For Textues:
      - IMG_Load_RW(SDL_RWops* path, 1);
    - For Sound:
      - MIX_LoadWAV_RW(SDL_RWops* path, 1);
      - MIX_LoadMUS_RW(SDL_RWops* path, 1);


PugiXML to load the path of the files


You can find some information in the [PhysFS Web](https://icculus.org/physfs/) or in the [PhysFS GitHub](https://github.com/icculus/physfs).


## Hand Out Tutorial
In the tutorial, follow the TODO's and use the links in the coments to reach the goals and extract files from Zip file

### [Releas Tutorial](https://github.com/Draquian/PhysFS_Roger_Salas/releases/tag/0.0)


## Hand Out Solution
In this solution you can find the .exe where if you executed, you can:
  - lisent a background music
  - see the image 1 and 2 (you can use tab to swap the image)
  - Play a SFX (use the key number 1 to l play the sound)

### [Releas Solution](https://github.com/Draquian/PhysFS_Roger_Salas/releases/tag/1.0)
