
#include <unistd.h>

#ifdef __APPLE__
    #include <mach/mach.h>
#endif

namespace Diagnostics{
    static void process_mem_usage(double& vm_usage, double& resident_set)
    {
       // Source - https://stackoverflow.com/a/671389
       // Posted by Don Wakefield, modified by community. See post 'Timeline' for change history
       // Retrieved 2026-08-05, License - CC BY-SA 2.5
       vm_usage = 0;
       resident_set = 0;
    
    #ifdef __linux__
       using std::ios_base;
       using std::ifstream;
       using std::string;
    
       vm_usage     = 0.0;
       resident_set = 0.0;
    
       // 'file' stat seems to give the most reliable results
       ifstream stat_stream("/proc/self/stat",ios_base::in);
    
       // dummy vars for leading entries in stat that we don't care about
       string pid, comm, state, ppid, pgrp, session, tty_nr;
       string tpgid, flags, minflt, cminflt, majflt, cmajflt;
       string utime, stime, cutime, cstime, priority, nice;
       string O, itrealvalue, starttime;
    
       // the two fields we want
       unsigned long vsize;
       long rss;
    
       stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
                   >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
                   >> utime >> stime >> cutime >> cstime >> priority >> nice
                   >> O >> itrealvalue >> starttime >> vsize >> rss; // don't care about the rest
    
       stat_stream.close();
    
       long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
       vm_usage     = vsize / 1024.0;
       resident_set = rss * page_size_kb;
    #endif
    
    #ifdef __APPLE__
        task_vm_info_data_t vm_info;
        mach_msg_type_number_t count = TASK_VM_INFO_COUNT;
    
        kern_return_t kr = task_info(
            mach_task_self(),
            TASK_VM_INFO,
            (task_info_t)&vm_info, 
            &count
        );
    
        if(kr == KERN_SUCCESS){
           vm_usage  = vm_info.phys_footprint;
        }
    #endif
    }

}
