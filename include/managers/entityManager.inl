template <typename E>
inline uint32 EntityManager<E>::GlobalEntityCount{ 0 };

template <typename E>
inline EntityManager<E>::~EntityManager()
{
    std::cout << "---------------[EntityManager]----------------" << std::endl;
    std::cout << "Releasing Entities... Cleanup   [Total: " << GlobalEntityCount << "]" << std::endl;
    uint32 deletedEntityCount{ Clear() };
    std::cout << "Entities Cleanup Completed    [Deleted: " << deletedEntityCount << "]" << std::endl;
    std::cout << "----------------------------------------------" << std::endl;
}

template <typename E>
inline E* EntityManager<E>::CreateEntity()
{
    static_assert(std::is_base_of<Entity2D, E>::value, "The E type MUST be an Entity");

    E* entity{ new E };
    entity->RootComponent = ComponentManager::CreateComponent<USceneComponent2D>(entity);
    entity->Components.push_back(entity->RootComponent);
    entity->Init_Internal(CurrentID++);
    ++GlobalEntityCount;

    Entities.push_back(entity);

    return entity;
}

template <typename E>
inline bool EntityManager<E>::RemoveEntity(E* entity)
{
    static_assert(std::is_base_of<Entity2D, E>::value, "The E type MUST be an Entity2D");

    TList<Entity2D*>::iterator it;
    for(it = Entities.begin(); it != Entities.end(); ++it)
    {
        if((*it)->ID == entity->ID)
        {
            (*it)->Destroy();
            delete (*it);
            Entities.erase(it);
            return true;
        }
    }
    return false;
}

template <typename E>
inline uint32 EntityManager<E>::Clear()
{
    static_assert(std::is_base_of<Entity2D, E>::value, "The E type MUST be an Entity2D");

    TList<Entity2D*>::iterator it;
    uint32 deletedEntityCount{ 0 };
    for(it = Entities.begin(); it != Entities.end(); ++it)
    {
        (*it)->Destroy();
        delete (*it);
        ++deletedEntityCount;
    }
    Entities.clear();

    return deletedEntityCount;
}

template <typename E>
inline void EntityManager<E>::PrintEntities()
{
    TList<Entity2D*>::iterator it;
    for(it = Entities.begin(); it != Entities.end(); ++it)
    {
        (*it)->Print();
    }
}
