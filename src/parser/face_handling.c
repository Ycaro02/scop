#include "../../include/scop.h"

/**
 * @brief Display face list
 * @param face_lst face list to display
*/
static void display_obj_face_lst(t_list *face_lst) {
	for (t_list *current = face_lst; current; current = current->next) {
		t_obj_face	*face = current->content;
		t_list		*curr_idx = face->idx;
		ft_printf_fd(1, CYAN"\nFace %u\n"RESET, face->id);
		for (t_list *curr_vertex = face->vertex; curr_vertex; curr_vertex = curr_vertex->next) {
			vec3_f32 *vec = curr_vertex->content;
			void *idx = curr_idx->content;
			ft_printf_fd(1, PINK"Vertex [%u] %f %f %f\n"RESET, *((u32 *)idx), (*vec)[0], (*vec)[1], (*vec)[2]);
			curr_idx = curr_idx->next;
		}
		ft_printf_fd(1, CYAN"ID: %u, Size %u\n"RESET, face->id, face->size);
	}
}

/**
 * @brief Extract face data
 * @param model obj model
 * @param face obj face
 * @param tri_idx triangle index
 * @return TRUE if success, FALSE otherwise (malloc failed)
*/
static u8 extract_face_data(t_obj_model *model, t_obj_face *face, u32 tri_idx) {
	vec3_u32	*tri_face = model->tri_face;

	for (u32 i = 0; i < 3; i++) {
		vec3_f32 *vertex = ft_calloc(sizeof(vec3_f32), 1);
		if (!vertex) {
			ft_printf_fd(2, RED"Error: Malloc failed\n"RESET);
			return (FALSE);
		}
		ft_vec_copy(*vertex, model->vertex[tri_face[tri_idx][i]], sizeof(vec3_f32));
		ft_lstadd_back(&face->vertex, ft_lstnew(vertex));
		u32 *idx = ft_calloc(sizeof(u32), 1);
		if (!idx) {
			ft_printf_fd(2, RED"Error: Malloc failed\n"RESET);
			return (FALSE);
		}
		*idx = tri_face[tri_idx][i];
		ft_lstadd_back(&face->idx, ft_lstnew(idx));
	} /* End for loop 3 */
	return (TRUE);
}

/**
 * @brief Increment tri_idx and face_size
 * @param tri_idx triangle index to increment by 1
 * @param face_size face size to increment by 3 (3 vertex per triangle)
*/
FT_INLINE void increment_tri_idx(u32 *tri_idx, u32 *face_size) {
	*tri_idx += 1;
	*face_size += 3;
}

t_list *get_all_face_vertex(t_obj_file *file, t_obj_model *model)
{
	t_list 		*face_lst = NULL;
	u32			face_id = 1, tri_idx = 0;

	/* Need to loop on face lst to identify different face, and loop on tri_face to select right, vertex and idx
		- loop on face list logic
			- NODE = face list content, get the 3 first vertex/index in tri_face, cause face is minimum triangle (3 vertex)
			- if NODE->other is not null, get the next 3 vertex/index in tri_face, get the next triangle
	*/

	for (t_list *current = file->face; current; current = current->next) {
		t_face_node	*curr_node = current->content;
		t_obj_face	*face = ft_calloc(sizeof(t_obj_face), 1);
		u32			face_size = 0;
		
		if (!face) {
			ft_printf_fd(2, RED"Error: Malloc failed\n"RESET);
			return (NULL);
		}
		face->id = face_id;
		extract_face_data(model, face, tri_idx);
		increment_tri_idx(&tri_idx, &face_size);
		if (curr_node->other) {
			extract_face_data(model, face, tri_idx);
			increment_tri_idx(&tri_idx, &face_size);
		}
		face->size = face_size;
		ft_lstadd_back(&face_lst, ft_lstnew(face));
		face_id++;
	}

	/* Display data here */
	display_obj_face_lst(face_lst);

	return (face_lst);
}
