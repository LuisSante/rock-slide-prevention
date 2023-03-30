// Component Registration Base
// ---------------------------
inline uint32 ComponentManager::ComponentID{ 0 };
inline uint32 ComponentManager::GlobalComponentCount{ 0 };
inline TArray<TList<BaseComponent*>> ComponentManager::ComponentBuffers{};
inline ComponentManagerGarbageCollector ComponentManager::GarbageCollector{};

inline uint32 ComponentManager::RegisterComponent()
{
    ComponentBuffers.push_back(TList<BaseComponent*>());
    return ComponentID++;
}


// Component Registration Process
// ---------------------------
template <typename T>
inline const uint32 Component<T>::ID{ ComponentManager::RegisterComponent() };

template <typename T>
inline const uint32 Component<T>::SIZE{ static_cast<uint32>(sizeof(T)) };


// Component Manager
// ---------------------------
inline ComponentManagerGarbageCollector::~ComponentManagerGarbageCollector()
{
    std::cout << "--------------[ComponentManager]--------------" << std::endl;
    std::cout << "Releasing Components... Cleanup [Total: " << ComponentManager::GlobalComponentCount << "]" << std::endl;
    uint32 deletedComponentCount{ 0 };
    for (TList<BaseComponent*>& buffer : ComponentManager::ComponentBuffers)
    {
        TList<BaseComponent*>::iterator it;
        for (it = buffer.begin(); it != buffer.end(); ++it)
        {
            delete (*it);
            ++deletedComponentCount;
        }
        buffer.clear();
    }
    std::cout << "Components Cleanup Completed  [Deleted: " << deletedComponentCount << "]" << std::endl;
    std::cout << "----------------------------------------------" << std::endl;
}

template <typename C>
inline C* ComponentManager::CreateComponent(Entity2D* entity)
{
    static_assert(std::is_base_of<BaseComponent, C>::value, "[THE C TYPE MUST BE A *COMPONENT*]");

    C* component{ new C };
    component->OwnerEntity = entity;
    ComponentBuffers[C::ID].push_front(component);
    ++GlobalComponentCount;

    return component;
}

template <typename C>
inline bool ComponentManager::DeleteComponent(Entity2D* entity)
{
    static_assert(std::is_base_of<BaseComponent, C>::value, "[THE C TYPE MUST BE A *COMPONENT*]");

    TList<BaseComponent*>::iterator it;
    for (it = ComponentBuffers[C::ID].begin(); it != ComponentBuffers[C::ID].end(); ++it)
    {
        if ((*it)->OwnerEntity->ID == entity->ID)
        {
            delete (*it);
            ComponentBuffers[C::ID].erase(it);
            return true;
        }
    }
    return false;
}

template <typename C>
inline TList<BaseComponent*>& ComponentManager::GetComponentBuffer()
{
    static_assert(std::is_base_of<BaseComponent, C>::value, "[THE C TYPE MUST BE A *COMPONENT*]");

    return ComponentBuffers[C::ID];
}
