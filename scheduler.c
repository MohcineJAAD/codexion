#include "codexion.h"

void	ft_release(t_dongle *dg, t_simulation *sim)
{
	if (strcmp(sim->scheduler, "fifo") == 0)
		ft_fifo_release(dg);
	else
		ft_edf_release(dg);
}
int		ft_acquire(t_coder *cd, t_dongle *dg)
{
	if(strcmp(cd->sim->scheduler, "fifo") == 0)
		return (ft_fifo_acquire(cd, dg));
	else
		return (ft_edf_acquire(cd, dg));
}
int ft_init_scheduler(t_environment *env)
{
    if (strcmp(env->sm->scheduler, "fifo") == 0)
        return (ft_init_fifo(env));
    else
        return (ft_init_edf(env));
}
