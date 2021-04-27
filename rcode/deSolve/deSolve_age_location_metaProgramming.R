library(deSolve)
library(ggplot2)
library(tidyr)
library(rlang)

discrete_p <- "/home/thinh/Dropbox/oucru/cpp/cpp_training/output/age_location"
outp <- "/home/thinh/Dropbox/oucru/cpp/plots"

days <- 20

# Time step
timeStep <- 0.001
timeStart <- 1 / timeStep + 2
timeEnd <- days / timeStep + 2

sir_equations_maker <- function(functionName, contactMatrix, ...) {
  # Make all combinations of groups
  groupGrid <- expand.grid(...)
  groupGrid$group <- apply(groupGrid, 1, function(x) paste(x, collapse = "_"))
  groupList <- list()
  
  # Paste S, I and R to these groups
  compartments <- c("S", "I", "R")
  for (i in 1:length(groupGrid$group)) {
    groupList[[groupGrid$group[i]]] <- paste(compartments, groupGrid$group[i], sep = "_")
  }
  
  # Make the names of all essential parameters
  infectiousComps <- sapply(groupList, function(x) x[2])
  popComps <- gsub("^I", "N", infectiousComps)
  betaParams <- paste0("beta_", groupGrid$group)
  gammaParams <- paste0("gamma_", groupGrid$group)
  foiParams <- paste0("forceInfection_", groupGrid$group)
  
  # Generate strings that are going to be parse as equation
  funBody <- c()
  funBodyReturn <- c()
  for (i in 1:length(infectiousComps)) {
    funBody <- append(funBody, paste0(foiParams[i], " <- ", betaParams[i], " * (", 
                                      paste0(contactMatrix[i,], " * ", infectiousComps, 
                                             " / ", popComps, collapse = " + "),
                                      ")"))
    funBody <- append(funBody, paste0("d", groupList[[i]][1], " <- -", foiParams[i], " * ", groupList[[i]][1]))
    funBody <- append(funBody, paste0("d", groupList[[i]][2], " <- ", foiParams[i], " * ", groupList[[i]][1], " - ", gammaParams[i], " * ", groupList[[i]][2]))
    funBody <- append(funBody, paste0("d", groupList[[i]][3], " <- ", gammaParams[i], " * ", groupList[[i]][2]))
    funBodyReturn <- append(funBodyReturn, paste0("d", groupList[[i]][1]))
    funBodyReturn <- append(funBodyReturn, paste0("d", groupList[[i]][2]))
    funBodyReturn <- append(funBodyReturn, paste0("d", groupList[[i]][3]))
  }
  
  funBody <- append(funBody, paste0("return(list(c(", paste(funBodyReturn, collapse = ", "), ")))"))
  
  # Generate sir equation function
  eval(parse(text = paste0(rlang::enexpr(functionName), " <- function(time, variables, parameters) {",
                           "with(as.list(c(variables, parameters)), {",
                           paste(funBody, collapse = "; "),
                           "})",
                           "}")), 
       envir = .GlobalEnv)
}

ageGroup <- c("0_14", "15_64", "65")
locGroup <- c("HCM", "HN")

age_matrix <- matrix(c(0.5, 0.6, 0.7, 
                       0.6, 0.4, 0.3, 
                       0.7, 0.3, 0.8), nrow = 3)
loc_matrix <- matrix(c(0.85, 0.1,
                       0.1, 0.95), nrow = 2)

# A full contact matrix computed by Kronecker product of these two matrices
contact_matrix <- kronecker(loc_matrix, age_matrix)

# Make sir_equations
sir_equations_maker(sir_equations, contact_matrix, ageGroup, locGroup)

# Input parameters
parameters_values <- c(
  beta_0_14_HCM = 1.5,
  beta_15_64_HCM = 1.5,
  beta_65_HCM = 1.5,
  beta_0_14_HN = 1.5,
  beta_15_64_HN = 1.5,
  beta_65_HN = 1.5,
  
  gamma_0_14_HCM = 0.5,
  gamma_15_64_HCM = 0.5,
  gamma_65_HCM = 0.5,
  gamma_0_14_HN = 0.5,
  gamma_15_64_HN = 0.5,
  gamma_65_HN = 0.5,
  
  N_0_14_HCM = 1000,
  N_15_64_HCM = 2000,
  N_65_HCM = 999,
  N_0_14_HN = 899,
  N_15_64_HN = 699,
  N_65_HN = 799
)

# Input initial values
initial_values <- c(
  # HCM
  S_0_14_HCM = 999,
  I_0_14_HCM =   1,
  R_0_14_HCM =   0,
  S_15_64_HCM = 1999,
  I_15_64_HCM =   1,
  R_15_64_HCM =   0,
  S_65_HCM = 999,
  I_65_HCM =   0,
  R_65_HCM =   0,
  
  # HN
  S_0_14_HN = 899,
  I_0_14_HN =   0,
  R_0_14_HN =   0,
  S_15_64_HN = 699,
  I_15_64_HN =   0,
  R_15_64_HN =   0,
  S_65_HN = 799,
  I_65_HN =   0,
  R_65_HN =   0
)

# Input time values
time_values <- seq(0, days) # days

# Solve the differential equations
sir_values_1 <- ode(
  y = initial_values,
  times = time_values,
  func = sir_equations,
  parms = parameters_values
)

sir_values_1
