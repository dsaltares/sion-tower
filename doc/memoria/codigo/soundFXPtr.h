class SoundFXPtr: public Ogre::SharedPtr<SoundFX> {
    public:
        SoundFXPtr(): Ogre::SharedPtr<SoundFX>() {}
        explicit SoundFXPtr(SoundFX* s): Ogre::SharedPtr<SoundFX>(s) {}
        SoundFXPtr(const SoundFXPtr& s): Ogre::SharedPtr<SoundFX>(s) {}
        SoundFXPtr(const Ogre::ResourcePtr& r);
        SoundFXPtr& operator= (const Ogre::ResourcePtr& r);
};
