void listAll(const MenuItem* m, string path) // two-parameter overload
{
    if (m == nullptr)
        return;
    
    if (m->menuItems() == nullptr)
    {
        cout << path + m->name() << endl;
        return;
    }
    else
    {
        if (m->name() != "")
        {
            path += m->name();
            cout << path << endl;
            path += "/";
        }
        
        for (vector<MenuItem*>::const_iterator it = m->menuItems()->begin(); it != m->menuItems()->end(); it++)
        {
            listAll(*it, path);
        }
    }
}