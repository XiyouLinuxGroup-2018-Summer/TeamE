switch (param) {
  case 1:if((dir = opendir(argv[2]))==NULL)  {
    my_err("opendir",__LINE__);
    exit(1);
  }
  while((ptr = readdir(dir))!=NULL){
          if(lstat(ptr->d_name,&buf)==-1){
        my_err("stat",__LINE__);
      }
      display_single(ptr->d_name);
      //display_attribute(buf,ptr->d_name);
  };break;
  case 2:if((dir = opendir(argv[2]))==NULL)  {
    my_err("opendir",__LINE__);
    exit(1);
  }
  while((ptr = readdir(dir))!=NULL){
          if(lstat(ptr->d_name,&buf)==-1){
        my_err("stat",__LINE__);
      }
      //display_single(ptr->d_name);
      display_attribute(buf,ptr->d_name);
  }break;
  default:break;
}

}else {
if((dir = opendir(argv[1]))==NULL)  {
  my_err("opendir",__LINE__);
  exit(1);
}
while((ptr = readdir(dir))!=NULL){
        if(lstat(ptr->d_name,&buf)==-1){
      my_err("stat",__LINE__);
    }
    display_single(ptr->d_name);
    //display_attribute(buf,ptr->d_name);
}


}
