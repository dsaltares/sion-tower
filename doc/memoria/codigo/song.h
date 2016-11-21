class Song: public Ogre::Resource {
    public:

        Song(Ogre::ResourceManager* creator,
             const Ogre::String& name,
             Ogre::ResourceHandle handle,
             const Ogre::String& group,
             bool isManual = false,
             Ogre::ManualResourceLoader* loader = 0);

        ~Song();


        void play(int loop = -1);
        void pause();
        void stop();
        void fadeIn(int ms, int loop = -1);
        void fadeOut(int ms);

        static bool isPlaying();
    protected:
        void loadImpl();
        void unloadImpl();

        size_t calculateSize() const;

    private:
        Mix_Music* _song;
        Ogre::String _path;
        size_t _size;
};
