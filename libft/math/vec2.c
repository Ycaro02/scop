# include "../libft.h"
# include "vec2.h"

/* @brief display vector list */
void display_lst_vec2(t_list *lst)
{
    t_list *tmp = lst;
    while(tmp)
    {
        t_vec2 *point = tmp->content;
        ft_printf_fd(1, "%spos : [%d][%d]%s\n",RED,point->y,point->x,RESET);
        tmp = tmp->next;
    }
}

/* @brief create a vector */
t_vec2 create_vector(uint32_t y, uint32_t x) {
        t_vec2 vec;
        vec.x = x;
        vec.y = y;
        return (vec);
}

/* @brief add vector */
t_vec2        add_vector(t_vec2 a, t_vec2 b) {
        a.x += b.x;
        a.y += b.y;
        return (a);
}

/* @brief sub vector */
t_vec2        sub_vector(t_vec2 a, t_vec2 b) {
        a.x -= b.x;
        a.y -= b.y;
        return (a);
}

/* @brief mult vector */
t_vec2        mult_vec2(t_vec2 a, t_vec2 b) {
        a.x *= b.x;
        a.y *= b.y;
        return (a);
}

/* @brief div vector */
t_vec2        div_vector(t_vec2 a, t_vec2 b) {
        a.x /= b.x;
        a.y /= b.y;
        return (a);
}

/**
 * @brief Compare two vector
 * @return 1 if equal, 0 if not
*/
int          vector_cmp(t_vec2 a, t_vec2 b) {
        if (a.y == b.y && a.x == b.x)
                return (1);
        return (0);
}


/**
 * @Get the absolute difference between vector and 2 int point
*/
// t_vec2        vector_abs_diff(t_vec2 a, t_vec2 b) {
//     t_vec2 diff = { 
// 		.y = abs_diff(a.y, b.y),
// 		.x = abs_diff(a.x, b.x)
// 	};
//     return (diff);
// }