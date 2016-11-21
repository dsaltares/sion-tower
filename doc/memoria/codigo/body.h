class Body {
    public:
        Body(GameObject* gameObject,
             const std::vector<Shape*>& shapes,
             const Ogre::Vector3& position = Ogre::Vector3::ZERO,
             const Ogre::Vector3& scale = Ogre::Vector3::UNIT_SCALE,
             const Ogre::Quaternion& orientation = Ogre::Quaternion::IDENTITY,
             int type = 0);
        
        Body(GameObject* gameObject = 0,
             const Ogre::Vector3& position = Ogre::Vector3::ZERO,
             const Ogre::Vector3& scale = Ogre::Vector3::UNIT_SCALE,
             const Ogre::Quaternion& orientation = Ogre::Quaternion::IDENTITY,
             int type = 0);

        ~Body();
        
        GameObject* getGameObject();
        void setGameObject(GameObject* gameObject);

        const std::vector<Shape*> getShapes() const;
        const std::vector<Shape*> getWorldSpaceShapes() const;

        void addShape(Shape* shape);
        bool removeShape(Shape* shape);
        bool removeShape(const Ogre::String& name);

        Ogre::Matrix4 getTransform() const;
        void setTransform(const Ogre::Matrix4& transform);
        const Ogre::Vector3& getPosition() const;
        void setPosition(const Ogre::Vector3& position);
        const Ogre::Vector3& getScale() const;
        void setScale(const Ogre::Vector3& scale);
        const Ogre::Quaternion& getOrientation() const;
        void setOrientation(const Ogre::Quaternion& orientation);
        
        int getType() const;
        void setType(int type);

        static bool getCollision(Body* bodyA, Body* bodyB);
    private:
        GameObject* _gameObject;
        std::vector<Shape*> _shapes;
        std::vector<Shape*> _worldShapes;
        Ogre::Vector3 _position;
        Ogre::Vector3 _scale;
        Ogre::Quaternion _orientation;
        int _type;

        void updateWorldShapes();
        void createWorldShapes();
};
