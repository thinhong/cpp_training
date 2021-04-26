library(rlang)

# Generate the name of all compartments
compartments <- c("S", "I", "R")
ageGroup <- c("0_14", "15_64", "65")
locGroup <- c("HCM", "HN")

groupGrid <- expand.grid(ageGroup, locGroup)
groupGrid$group <- apply(groupGrid, 1, function(x) paste(x, collapse = "_"))

groupList <- list()
for (i in 1:nrow(groupGrid)) {
  groupList[[i]] <- paste(compartments, groupGrid$group[i], sep = "_")
}

age_matrix <- matrix(c(0.5, 0.6, 0.7, 
                       0.6, 0.4, 0.3, 
                       0.7, 0.3, 0.8), nrow = 3)
loc_matrix <- matrix(c(0.85, 0.1,
                       0.1, 0.95), nrow = 2)
# A full contact matrix computed by Kronecker product of these two matrices
contact_matrix <- kronecker(loc_matrix, age_matrix)

# Extract infectious compartments = the second sub-element of every element in the list
infectiousComps <- sapply(groupList, function(x) x[2])
popComps <- gsub("^I", "N", infectiousComps)

betaParams <- paste0("beta_", groupGrid$group)
gammaParams <- paste0("gamma_", groupGrid$group)
foiParams <- paste0("forceInfection_", groupGrid$group)

command <- c()
for (i in 1:length(infectiousComps)) {
  command <- append(command, paste0(foiParams[i], " <- ", betaParams[i], " * (", 
         paste0(contact_matrix[i,], " * ", infectiousComps, 
                " / ", popComps, collapse = " + "),
         "); ",
         "d", groupList[[i]][1], " <- -", foiParams[i], " * ", groupList[[i]][1], "; ",
         "d", groupList[[i]][2], " <- ", foiParams[i], " * ", groupList[[i]][1], 
         " - ", gammaParams[i], " * ", groupList[[i]][2], "; ",
         "d", groupList[[i]][3], " <- ", gammaParams[i], " * ", groupList[[i]][2], "; "))
}
command
