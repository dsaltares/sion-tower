class Level {
    public:
        Level(const Ogre::String& id);
        ~Level();

        void load();
        void unload();
        bool isLoaded() const;
        
        const Ogre::String& getId() const;
        const Ogre::String& getName() const;
        const Ogre::String& getDescription() const;
        const Ogre::String& getSongName() const;

        SongPtr getSong();
        std::vector<GameMesh*>& getSceneObjects();
        const Ogre::Vector3& getPlayerPosition() const;
        std::vector<EnemySpawn>& getEnemySpawns();
        NavigationMesh* getNavigationMesh();
        
    private:
        // Informacion basica
        Ogre::String _id;
        Ogre::String _name;
        Ogre::String _description;
        Ogre::String _musicName;
        Ogre::String _musicGroup;
        bool _loaded;
        
        // Cancion
        SongPtr _song;
        
        // Elementos del juego y escenario
        Player* _player;
        std::vector<GameMesh*> _sceneObjects;
        std::vector<std::pair <Ogre::SceneNode*, Ogre::Light*> > _lights;
        std::vector<std::pair <Ogre::SceneNode*, Ogre::Entity*> > _geometry;
        std::vector<std::pair <Ogre::SceneNode*, Ogre::ParticleSystem*> > _particles;
        NavigationMesh* _navigationMesh;
        Ogre::Vector3 _playerPos;
        std::vector<EnemySpawn> _enemySpawns;
        
        void loadBasicInfo();

        void loadAmbientInfo(const pugi::xml_document& doc);
        void loadNodesInfo(const pugi::xml_document& doc);
        void loadEntity(const pugi::xml_node& node,
                        const Ogre::String& name,
                        const Ogre::Vector3& position,
                        const Ogre::Vector3& scale,
                        const Ogre::Quaternion& orientation);
        void loadLight(const pugi::xml_node& node,
                       const Ogre::String& name,
                       const Ogre::Vector3& position,
                       const Ogre::Vector3& scale,
                       const Ogre::Quaternion& orientation);
        void loadCamera(const pugi::xml_node& node,
                        const Ogre::Vector3& position,
                        const Ogre::Quaternion& orientation);
};
