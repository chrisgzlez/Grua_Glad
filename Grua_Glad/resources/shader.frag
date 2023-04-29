#version 330 core 

out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;


uniform sampler2D texture1;

void main()

{
	
	// Direccion de la Luz
	vec3 luz_dir = vec3(0,0,-1);
	
	
	// Luz Ambiente
	float ambiente_intensity = 1.0f;					// Intensidad luz ambiente
	vec3 ambient = ambiente_intensity * lightColor;		// Calculo Luz ambiente

	/// CREO QUE SOBRA //////////////////
	// Calcula el vector de direccion de la luz

	vec3 ld = normalize(-lightPos);				// Normaliza la posicion de la luz
	ld = luz_dir;								// Carga la direccion de la luz

	/////////////////////////////////////

	// Vector de direccion (de donde va a donde llega)
	vec3 fd = normalize(vec3((FragPos-lightPos)));

	/// Limita el ángulo de la Luz a 9 radianes (conico)
	if(acos(dot(fd,ld)) < radians(9)){

		/// DIFUMINAR
		vec3 norm = normalize(Normal);						// Cargar normal
		vec3 light_vector = normalize(lightPos - FragPos);	// Vector de direccion de la luz
		float diff = max(dot(norm, light_vector), 0.0);			// Calcula el factor de difuminacion
		vec3 diffuse = diff * lightColor;					// Aplica el factor de difuminacion al color de luz

		/// Specular
		float specular_intensity = 1.0;									// Intensidad
		vec3 viewDir = normalize(viewPos - FragPos);					// Vector de direccion de la vista
		vec3 reflectDir = reflect(-light_vector, norm);						// Vector de direcccion de la reflexion
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);		// Calculo del factor specular
		vec3 specular = specular_intensity * spec * lightColor;			// Aplica el factor specular, intensidad al color de luz

		/// RESULTADO
		// Luz final
		vec3 result = ((ambient + diffuse + specular) * objectColor)/2; // Combina la luz specular con la de ambiente y la difusa y el color del objeto

		FragColor = vec4(result, 0.75) * texture2D(texture1, TexCoord); // Aplica la luz al fragment
	}
	else{
	/// Fuera del rango del foco de luz
		vec3 result = ((ambient) * objectColor)/2;							// Calcula luz de ambiente
		FragColor = vec4(result, 0.75) *  texture2D(texture1, TexCoord);		// Aplica la luz al fragment
	}

}