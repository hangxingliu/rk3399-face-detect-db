#include "./firefly_faceverify_version.hpp"
#include "../types/firefly_face_sdk.hpp"

#ifndef FIREFLY_FACEVERIFY_HPP
#define FIREFLY_FACEVERIFY_HPP 1


#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
#    ifdef FF_EXPORTS
#        define FF_API  extern "C" __attribute__((visibility ("default")))
#    else
#        define FF_API extern "C"
#    endif
#else
#    ifdef FF_EXPORTS
#        define FF_API __attribute__((visibility ("default")))
#    else
#        define FF_API
#    endif
#endif

FF_API FF_Detector firefly_fd_init_detector();

FF_API void firefly_fd_deinit_detector(const FF_Verifier detector_handle);

FF_API int firefly_fd_detect_image(
	const FF_Detector detector_handle,
    const FF_Image image,
    FF_FaceInfo **p_faces_array,
	int *p_faces_count);

FF_API void firefly_fd_free_detect_result(const FF_FaceInfo* faces_array);

FF_API int firefly_fd_get_version();

FF_API FF_Verifier firefly_fv_init_verifier();

FF_API void firefly_fv_deinit_verifier(const FF_Verifier verifier_handle);

FF_API int firefly_fv_extract_feature(
	const FF_Verifier verifier_handle,
    const FF_Image image,
	const FF_FaceInfo face,
	FF_FaceFeatures* p_feature);

FF_API float firefly_fv_compare_feature(
	FF_FaceFeatures& feature1,
	FF_FaceFeatures& feature2);

FF_API int firefly_fv_get_feature_length(const FF_Verifier verifier);

FF_API int firefly_fv_get_version();

#ifdef __cplusplus
}
#endif

#endif
