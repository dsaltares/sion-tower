class SongPtr: public Ogre::SharedPtr<Song> {
    public:
        SongPtr(): Ogre::SharedPtr<Song>() {}
        explicit SongPtr(Song* m): Ogre::SharedPtr<Song>(m) {}
        SongPtr(const SongPtr &m): Ogre::SharedPtr<Song>(m) {}
        SongPtr(const Ogre::ResourcePtr &r);
        SongPtr& operator= (const Ogre::ResourcePtr& r);
};
