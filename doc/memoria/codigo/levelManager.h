class LevelManager: public Ogre::Singleton<LevelManager> {
    public:
    
        enum UnlockCode {
            ALREADYUNLOCKED,
            LASTLEVEL,
            UNLOCKED
        };
    
        LevelManager();
        ~LevelManager();

        static LevelManager& getSingleton();
        static LevelManager* getSingletonPtr();

        std::vector<Level*>& getLevels();
        Level* getLevel(const Ogre::String& id);
        int getNumLevels() const;

        Body* createBodyFromCatalog(const Ogre::String& name);
        
        UnlockCode tryUnlockLevel(const Ogre::String& code, int lastUnlockedLevel);
    private:
        std::vector<Level*> _levels;
        int _numLevels;

        typedef boost::unordered_map<Ogre::String, Body* > BodyCatalog;
        BodyCatalog _bodyCatalog;
        std::vector<Shape*> _shapes;

        void lookForLevels();
        void initialiseBodyCatalog();
};
