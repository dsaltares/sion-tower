class SongManager: public Ogre::ResourceManager,
                     public Ogre::Singleton<SongManager> {
    public:
        SongManager();
        virtual ~SongManager();

        virtual SongPtr load(const Ogre::String& name,
                             const Ogre::String& group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

        static SongManager& getSingleton();
        static SongManager* getSingletonPtr();

    protected:
        Ogre::Resource* createImpl(const Ogre::String& name,
                                   Ogre::ResourceHandle handle,
                                   const Ogre::String& group,
                                   bool isManual,
                                   Ogre::ManualResourceLoader* loader,
                                   const Ogre::NameValuePairList* createParams);
};
