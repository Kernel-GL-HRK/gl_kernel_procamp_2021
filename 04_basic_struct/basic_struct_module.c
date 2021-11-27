#include <linux/module.h>
#include <linux/init.h>
#include <linux/kobject.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/types.h>

// Useful link:
// https://elixir.bootlin.com/linux/latest/source/samples/kobject/kobject-example.c#L131

typedef struct string_list_item{
    struct list_head list;
    char* stored_string;
    size_t stored_string_length;
}string_list_item_t;


static struct kobject *list_obj;
static struct list_head string_list;

static ssize_t add_new_string_entity(const char* buffer)
{
    string_list_item_t* p_entity = kmalloc(sizeof(string_list_item_t),GFP_KERNEL);
    if(!p_entity)
        return -ENOMEM;
    
    const size_t string_length = strlen(buffer);
    const size_t string_buffer_mem_size = string_length+1;
    char* string_buffer = kmalloc(string_buffer_mem_size, GFP_KERNEL);
    if(!string_buffer){
        kfree(p_entity);
        return -ENOMEM;
    }
    p_entity->stored_string = string_buffer;

    copy_from_user(string_buffer,buffer,string_buffer_mem_size);
    p_entity->stored_string_length = string_length;

    list_add_tail(&p_entity->list,string_list);

    return 0;
}

static void string_item_dtor(string_list_item_t* string_list_item){
    if(!string_list_item)
        return;

    kfree(string_list->stored_string);
    string_list->stored_string = NULL;
    string_list->stored_string_length = 0;
    kfree(string_list_item);
}
static void cleanup_storage()
{
    string_list_item_t* string_it = NULL;
    struct list_head* pos, *q;
    list_for_each_safe(pos,q,&string_list)
    {
        string_it = list_entry(pos,string_list_item_t,list);
        list_del(pos);
        string_item_dtor(string_it);
    }

}
static ssize_t  list_show(struct kobject *kobj, 
                        struct kobj_attribute *attr, char *buf)
{
    size_t copied_data_size = 0;

    string_list_item_t* list_entity_it = NULL;
    list_for_each_entry(list_entity_it,&string_list,list)
    {
        size_t copied_data_size = list_entity_it->stored_string_length+1;
        copy_to_user(buf,list_entity_it->stored_string,copied_data_size);
        copied_data_size+=copied_data_size;
    }
    pr_debug("==BASIC_STRUCT==:Called " __func__ "with copied data size:%d", copied_data_size);
    return copied_data_size;
}

static ssize_t  list_store(struct kobject *kobj, 
                        struct kobj_attribute *attr,const char *buf, size_t count)
{
    pr_debug("==BASIC_STRUCT==:Called " __func__ "with count:%d", count);
    ssize_t res = add_new_string_entity(buf);
    if(!res)
        return count;
    return res;
}


struct kobj_attribute list_attribute = __ATTR(list, 0660, list_show, list_store);

static int __init simple_struct_module_init(void)
{
    int result = 0;
    list_obj = kobject_create_and_add("basuic_struct", kernel_kobj);
    if (!list_obj){
        pr_warn("%s:%d: kobject_create_and_add failed.\n", __func__,
			__LINE__);
		return -ENOMEM;
    }
    result = sysfs_create_file(list_obj, &list_attribute.attr);
    if(res){
        pr_warn("%s:%d: sysfs_create_file failed.\n", __func__,
			__LINE__);
        kobject_put(list_obj);
        return -ENOMEM;
    }
    INIT_LIST_HEAD(&string_list);

    pr_debug("==BASIC_STRUCT==:Module ready");
    return result;
}


static void __exit simple_struct_module_exit(void)
{
    cleanup_storage();

    kobject_put(list_obj);
    pr_debug("==BASIC_STRUCT==:Module unloaded");
}

module_init(simple_struct_module_init);
module_exit(simple_struct_module_exit);

MODULE_LICENSE("MIT");
MODULE_AUTHOR("Valentyn Korniienko");