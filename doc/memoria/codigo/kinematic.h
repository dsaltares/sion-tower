class Kinematic {
    public:
        Kinematic(const Ogre::Vector3& position = Ogre::Vector3::ZERO,
                  Ogre::Real orientation = 0.0f,
                  const Ogre::Vector3& velocity = Ogre::Vector3::UNIT_SCALE,
                  Ogre::Real rotation = 0.0f,
                  Ogre::Real maxSpeed = 0.0f);
        
        const Ogre::Vector3& getPosition() const;
        Ogre::Real getOrientation() const;
        const Ogre::Vector3& getVelocity() const;
        Ogre::Real getRotation() const;
        Ogre::Real getMaxSpeed() const;
        
        void setPosition(const Ogre::Vector3& position);
        void setOrientation(Ogre::Real orientation);
        void setVelocity(const Ogre::Vector3& velocity);
        void setRotation(Ogre::Real rotation);
        void setMaxSpeed(Ogre::Real maxSpeed);

        void update(const Steering& steering, Ogre::Real deltaT);
        
        void setOrientationFromVelocity();
        void lookAt(const Ogre::Vector3& point);
    private:
        Ogre::Vector3 _position;
        Ogre::Real _orientation;
        Ogre::Vector3 _velocity;
        Ogre::Real _rotation;
        Ogre::Real _maxSpeed;
};
