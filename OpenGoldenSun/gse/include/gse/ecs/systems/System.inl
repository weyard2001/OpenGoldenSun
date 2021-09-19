template <typename T>
void System::requireComponent() {
    m_componentMask.set(Component<T>::getID());
}