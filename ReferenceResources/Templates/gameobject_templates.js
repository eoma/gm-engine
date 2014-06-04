[
    {
    	"template": "Company",
    	"components": [ "Company" ]
    },
    {
    	"template": "CompanyInfo",
    	"components": [ "CompanyInfo"]
    },
    {
        "template": "Empty Air",
        "components": ["Tile"]
    },
	{
        "template": "World Tile",
        "components": ["WorldTile"]
    },
	{
        "template": "World Region Tile",
        "components": ["WorldTile", "Region"],
		"properties": [
			{
				"name": "Description",
				"value": "This is a region..."
			}
		]
    },
	{
        "template": "World Region Tile Easy",
        "requires": ["World Region Tile"],
		"properties": [
			{
				"name": "Cost",
				"value": 10,
				"type": "int"
			}
		]
    },
	{
        "template": "World Region Tile Medium",
        "requires": ["World Region Tile"],
		"properties": [
			{
				"name": "Cost",
				"value": 20,
				"type": "int"
			}
		]
    },
	{
        "template": "World Region Tile Hard",
        "requires": ["World Region Tile"],
		"properties": [
			{
				"name": "Cost",
				"value": 40,
				"type": "int"
			}
		]
    }
	
]
