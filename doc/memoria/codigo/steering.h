struct Steering {
    public:
        Steering(const Ogre::Vector3& linear = Ogre::Vector3::ZERO,
                 Ogre::Real angular = 0.0f);
    
        const Ogre::Vector3& getLinear() const;
        Ogre::Real getAngular() const;
        bool isNone() const;
        
        void setLinear(const Ogre::Vector3 linear);
        void setAngular(Ogre::Real angular);
        void setNone();
        
        
        // Operadores
        bool operator == (const Steering& other);
        Steering& operator + (const Steering& other);
        Steering& operator += (const Steering& other);
        Steering& operator * (Ogre::Real f);
        Steering& operator *= (Ogre::Real f);
        Steering& operator / (Ogre::Real f);
        Steering& operator /= (Ogre::Real f);
    
    private:
        Ogre::Vector3 _linear;
        Ogre::Real _angular;
};
