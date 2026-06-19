#include "codexion.h"

// static void ft_init_edf(t_environment *env)
// {
//     (void)env;
// }

void	ft_release(t_dongle *dg, t_simulation *sim)
{
	if (strcmp(sim->scheduler, "fifo") == 0)
		ft_fifo_release(dg);
	// TODO: ft_edf_release()
}
int		ft_acquire(t_coder *cd, t_dongle *dg)
{
	if(strcmp(cd->sim->scheduler, "fifo") == 0)
		return (ft_fifo_acquire(cd, dg));
	// TODO: ft_edf_acquire()
}
int ft_init_scheduler(t_environment *env)
{
    if (strcmp(env->sm->scheduler, "fifo") == 0)
        return (ft_init_fifo(env));
    else
        // TODO: ft_init_edf();
    return (1);
}
