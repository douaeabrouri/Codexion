
#include "codexion.h"

int check_the_input(char *str, int loop)
{
    size_t index = 0;
    size_t len = strlen(str);
    long value = 0;
    
    if (len == 0){
        printf("Invalid empty value in position %d\n", loop);
        return (-1);
    }
    while (index < len){
        if (str[index] >= '0' && str[index] <= '9'){
            value = value * 10 + (str[index] - '0');
            if (value > INT_MAX){
                printf("%s\n", "Invalid Value ");
                return (-1);
            }
            index++;
        }
        else{
            printf("Invalide value -> %c in position %d\n", str[index], loop);
            return (-1);
        }
    }
    return ((int)value);
}

t_data	*parsing(char **lst, int n_arg)
{
	t_data	*data;

	if (!check_arg_count(n_arg))
		return (NULL);
	if (!check_scheduler(lst[n_arg - 1]))
		return (NULL);
	data = malloc(sizeof(t_data));
	if (data == NULL)
		return (NULL);
	if (!parse_values(data, lst, n_arg))
	{
		free(data);
		return (NULL);
	}
	set_scheduler(data, lst[n_arg - 1]);
	return (data);
}
