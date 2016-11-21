class SoundFX: public Ogre::Resource {
    public:
        SoundFX(Ogre::ResourceManager* creator,
               const Ogre::String& name,
               Ogre::ResourceHandle handle,
               const Ogre::String& group,
               bool isManual = false,
               Ogre::ManualResourceLoader* loader = 0);


        ~SoundFX();


        int play(int loop = 0);
        
    protected:
        void loadImpl();
        void unloadImpl();
        size_t calculateSize() const;
        
    private:
        Mix_Chunk* _sound;
        Ogre::String _path;
        size_t _size;
};
