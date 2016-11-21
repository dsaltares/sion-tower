class State {
    public:
        // Constructor y destructor
        State(StateManager* stateManager);
        virtual ~State() {};
       
        void setStateManager(StateManager* stateManager);
        
        
        // Cargar y liberar escena
        virtual void load() {};
        virtual void clear() {};
        
        // Gestores de eventos
        virtual void update(Ogre::Real deltaT, bool active) = 0;
        virtual bool keyPressed(const OIS::KeyEvent &arg);
        virtual bool keyReleased(const OIS::KeyEvent &arg);
        virtual bool mouseMoved(const OIS::MouseEvent &arg);
        virtual bool mousePressed(const OIS::MouseEvent &arg,
                                  OIS::MouseButtonID id);
        virtual bool mouseReleased(const OIS::MouseEvent &arg,
                                   OIS::MouseButtonID id);

    protected:
        // Log
        Ogre::Log* _log;

        // Gestor escena
        Ogre::SceneManager* _sceneManager;
        
        StateManager* _stateManager;
        bool _loaded;
        
        // Obliga a traducir todos los estados
        virtual void translate() = 0;
        
        // Obliga a ajustar el tamano del texto
        virtual void adjustFontHeight() = 0;
};
