void listAll(string path, const Domain* d) // two-parameter overload
{
    // no subdomains, so just print the current domain
    if(d->subdomains().empty())
        cout << path << endl;
    
    for(int i = 0; i < d->subdomains().size(); i++){
        // no "." proceeding empty string
        if(path == "")
            listAll(d->subdomains()[i]->label() + path, d->subdomains()[i]);
        // print the subdomains, then a comma, and the current domain
        else
            listAll(d->subdomains()[i]->label() + "." + path, d->subdomains()[i]);
    }
}
